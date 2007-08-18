#include "pch.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdorss.h"
#include "rdopat.h"
#include "rdofunc.h"
#include "rdodpt.h"
#include "rdoparser_lexer.h"
#include <rdo_runtime.h>
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace rdoParse 
{

int funlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void funerror( char* mes )
{
}

const RDOFUNFunctionParam* const RDOFUNFunction::findFUNFunctionParam( const std::string& paramName ) const 
{
	std::vector< const RDOFUNFunctionParam* >::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName2<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _retType ):
	RDOParserObject( _parser ),
	name( _name ),
	retType( _retType )
{
	parser->insertFUNFunction( this );
}

int RDOFUNFunction::findFUNFunctionParamNum( const std::string& paramName ) const
{
	std::vector<const RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName2<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return -1;

	return it - params.begin();
}

void RDOFUNFunction::add( const RDOFUNFunctionParam* const _param )
{ 
	if ( findFUNFunctionParam( _param->getName() ) ) {
		parser->lexer_loc_set( _param->src_pos().last_line, _param->src_pos().last_pos );
		parser->error( rdo::format("Параметр уже существует: %s", _param->getName().c_str()) );
//		parser->error("Second appearance of the same parameter name: " + *(_param->getName()));
	}
	params.push_back( _param ); 
}

void RDOFUNFunction::add(const RDOFUNFunctionListElement *const _param)
{
	listElems.push_back(_param); 
}

void RDOFUNFunction::add( const RDOFUNCalculateIf* const _calculateIf )
{
	calculateIf.push_back( _calculateIf );
}

void RDOFUNFunction::createListCalc()
{
	int numParams = params.size();
	int elements = listElems.size();
	int currElement = 0;

	if ( !retType->dv->isExist() ) {
		parser->error( rdo::format("Функция '%s' должна иметь значение по-умолчанию", name.c_str()) );
//		parser->error(("list function " + *name + " must have default result value").c_str());
	}

	rdoRuntime::RDOCalcConst*   defaultValue = new rdoRuntime::RDOCalcConst( parser->runTime, retType->getParamDefaultValue() );
	rdoRuntime::RDOFunListCalc* funCalc      = new rdoRuntime::RDOFunListCalc( parser->runTime, defaultValue );
	parser->lexer_loc_backup();
	try {
		for(;;)	// for all cases in list
		{
			if ( currElement >= elements )
				break;

			rdoRuntime::RDOCalc* caseCalc = new rdoRuntime::RDOCalcConst(parser->runTime, true);
			for ( int currParam = 0; currParam < numParams; currParam++ ) {
				if ( currElement >= elements ) {
					parser->error( "Указаны значения не всех параметров" );
				}
				const RDOFUNFunctionListElement* const arg = listElems.at( currElement++ );
				if ( arg->isEquivalence() ) {
					parser->lexer_loc_set( arg->src_pos().first_line, arg->src_pos().first_pos );
					parser->error( "Указаны значения не всех параметров" );
				}
				parser->lexer_loc_set( arg->src_pos().last_line, arg->src_pos().last_pos );
				const RDOFUNFunctionParam *const param = params.at(currParam);
				rdoRuntime::RDOCalcFuncParam *funcParam = new rdoRuntime::RDOCalcFuncParam(parser->runTime, currParam);
				rdoRuntime::RDOCalcIsEqual *compareCalc = arg->createIsEqualCalc(param, funcParam);
				rdoRuntime::RDOCalc *andCalc = new rdoRuntime::RDOCalcAnd(parser->runTime, caseCalc, compareCalc);
				caseCalc = andCalc;
			}

			if ( currElement >= elements ) {
				parser->error( "Ожидается '= <значение_функции>'" );
			}
			const RDOFUNFunctionListElement* const eq = listElems.at( currElement++ );
			if ( !eq->isEquivalence() ) {
				parser->lexer_loc_set( eq->src_pos().first_line, eq->src_pos().first_pos );
				parser->error( "Ожидается '= <значение_функции>'" );
			}

			if ( currElement >= elements ) {
				parser->lexer_loc_set( eq->src_pos().last_line, eq->src_pos().last_pos );
				parser->error( "Ожидается '<значение_функции>'" );
			}
			const RDOFUNFunctionListElement* const res = listElems.at(currElement++);
			parser->lexer_loc_set( res->src_pos().last_line, res->src_pos().last_pos );
			rdoRuntime::RDOCalcConst* resultCalc = res->createResultCalc(retType);

			funCalc->addCase(caseCalc, resultCalc);
		}
		functionCalc = funCalc;
		parser->lexer_loc_restore();

	} catch ( std::out_of_range ex ) {
		parser->error( rdo::format( "Неверное количество элементов функции: %s", name.c_str()) );
//		parser->error(("Wrong element number in list function " + *name).c_str());
	}
}

void RDOFUNFunction::createTableCalc()
{
	parser->lexer_loc_backup();
	int numParams = params.size();
	try {
		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcConst(parser->runTime, 0);
		int d = 1;
		for ( int currParam = 0; currParam < numParams; currParam++ ) {
			const RDOFUNFunctionParam* const param  = params.at(currParam);
			parser->lexer_loc_set( param->src_pos().last_line, param->src_pos().last_pos );
			rdoRuntime::RDOCalcFuncParam *funcParam = new rdoRuntime::RDOCalcFuncParam(parser->runTime, currParam);
			rdoRuntime::RDOCalc *val2 = funcParam;
			if ( param->getType()->getType() != RDORTPParamType::pt_enum ) {
				rdoRuntime::RDOCalcConst *const1 = new rdoRuntime::RDOCalcConst(parser->runTime, 1);
				val2 = new rdoRuntime::RDOCalcMinus(parser->runTime, val2, const1);
			}
			rdoRuntime::RDOCalcConst *const2 = new rdoRuntime::RDOCalcConst(parser->runTime, d);
			rdoRuntime::RDOCalcMult *mult = new rdoRuntime::RDOCalcMult(parser->runTime, const2, val2);
			rdoRuntime::RDOCalcPlus *add = new rdoRuntime::RDOCalcPlus(parser->runTime, mult, calc);
			d *= param->getType()->getDiapTableFunc();
			calc = add;
		}

		if ( listElems.size() != d ) {
			parser->lexer_loc_restore();
			parser->error( rdo::format("Неверное количество элементов табличной функции '%s': %d, требуется %d", name.c_str(), listElems.size(), d) );
//			parser->error(("wrong number of value in table function " + *name).c_str());
		}

		rdoRuntime::RDOFuncTableCalc *funCalc = new rdoRuntime::RDOFuncTableCalc(parser->runTime, calc);
		for ( int currElem = 0; currElem < d; currElem++ ) {
			const RDOFUNFunctionListElement* const el = listElems.at(currElem);
			parser->lexer_loc_set( el->src_pos().last_line, el->src_pos().last_pos );
			if ( el->isEquivalence() ) {
				parser->error( "Символ '=' недопустим в табличной функции" );
//				parser->error("\"=\" unexpected in table function");
			}
			rdoRuntime::RDOCalcConst* resultCalc = el->createResultCalc(retType);
			funCalc->addResultCalc(resultCalc);
		}
		functionCalc = funCalc;
		parser->lexer_loc_restore();

	} catch ( std::out_of_range ex ) {
		parser->error( rdo::format("Неверное количество элементов табличной функции '%s'", name.c_str()) );
//		parser->error(("Wrong element number in list function " + *name).c_str());
	}
}

rdoRuntime::RDOCalcIsEqual *RDOFUNFunctionListElement::createIsEqualCalc(const RDOFUNFunctionParam *const param, const rdoRuntime::RDOCalcFuncParam *const funcParam) const
{
	rdoRuntime::RDOCalcConst *constCalc = createResultCalc(param->getType());
	rdoRuntime::RDOCalcIsEqual *res = new rdoRuntime::RDOCalcIsEqual(parser->runTime, constCalc, funcParam);
	return res;
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementIdentif::createResultCalc(const RDORTPParamType *const retType) const
{
	return new rdoRuntime::RDOCalcConst(parser->runTime, retType->getRSSEnumValue(value));
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementReal::createResultCalc(const RDORTPParamType *const retType) const 
{
	return new rdoRuntime::RDOCalcConst(parser->runTime, retType->getRSSRealValue(value));
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementInt::createResultCalc(const RDORTPParamType *const retType) const 
{
	return new rdoRuntime::RDOCalcConst(parser->runTime, retType->getRSSIntValue(value));
}

rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementEq::createResultCalc( const RDORTPParamType* const retType ) const
{
	parser->error( "Внутренная ошибка парсера: неверное состояние" );
//	parser->error( "Internal parser error, incorrect state" );
	return NULL;	// unreachable code
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic( rdoRuntime::RDOCalc *_calc ):
	calc( _calc )
{
	if ( calc ) calc->setSrcFileType( src_filetype() );
}

RDOFUNLogic* RDOFUNLogic::operator &&( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcAnd( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator ||( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcOr( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator_not()
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcNot( parser->runTime, calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	return logic;
}

void RDOFUNLogic::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( calc ) calc->setSrcInfo( src_info );
}

void RDOFUNLogic::setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end )
{
	RDOParserSrcInfo::setSrcInfo( begin, delim, end );
	if ( calc ) calc->setSrcInfo( begin, delim, end );
}

void RDOFUNLogic::setSrcPos( const YYLTYPE& _error_pos )
{
	RDOParserSrcInfo::setSrcPos( _error_pos );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNLogic::setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
{
	RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNLogic::setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
{
	RDOParserSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
	if ( calc ) calc->setSrcPos( first_line, first_pos, last_line, last_pos );
}

void RDOFUNLogic::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( calc ) calc->setSrcText( value );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, RDORTPParamType::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parser ),
	type( _type ),
	enu( NULL ),
	str( "" ),
	calc( _calc )
{
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, RDORTPParamType::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parent ),
	type( _type ),
	enu( NULL ),
	str( "" ),
	calc( _calc )
{
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( res_name_src_info, par_name_src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( res_name_src_info, par_name_src_info );
}

void RDOFUNArithm::init( const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info )
{
	setSrcInfo( res_name_src_info, ".", par_name_src_info );
	const RDORSSResource* const res = parser->findRSSResource( res_name_src_info.src_text() ); 
	if ( res ) {
		// Это ресурс с закладки RSS
		if ( res->getType()->isTemporary() ) {
			parser->error( res_name_src_info, rdo::format("Нельзя использовать временный ресурс: %s", res_name_src_info.src_text().c_str()) );
//			parser->error(("Cannot use temporary resource in function: " + *res_name_src_info.src_text()).c_str());
		}
		int resNumb = res->getNumber();
		int parNumb = res->getType()->getRTPParamNumber( par_name_src_info.src_text() );
		if ( parNumb == -1 ) {
			parser->lexer_loc_set( par_name_src_info.src_pos().last_line, par_name_src_info.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
		}
		calc = new rdoRuntime::RDOCalcGetResParam( parser->runTime, resNumb, parNumb );
		calc->setSrcInfo( src_info() );
		type = res->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = ((RDORTPEnumParamType *)res->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
		}
		return;
	} else {
		// Это не ресурс, но возможно релевантный ресурс или ресурс, внутри групповой функции
		if ( !parser->getFUNGroupStack().empty() && parser->getFUNGroupStack().back()->resType->getName() == res_name_src_info.src_text() ) {
			// Это ресурс внутри групповой функции
			RDOFUNGroup* currGroup = parser->getFUNGroupStack().back();
			int parNumb = currGroup->resType->getRTPParamNumber( par_name_src_info.src_text() );
			if ( parNumb == -1 ) {
				parser->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
			}
			calc = new rdoRuntime::RDOCalcGetGroupResParam( parser->runTime, parNumb );
			calc->setSrcInfo( src_info() );
			type = currGroup->resType->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
			if ( type == RDORTPParamType::pt_enum ) {
				enu = ((RDORTPEnumParamType *)currGroup->resType->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
			}
			return;
		} else if ( parser->getFileToParse() == rdoModelObjects::PAT && parser->getLastPATPattern() && parser->getLastPATPattern()->findRelevantResource( res_name_src_info.src_text() ) ) {
			// Это релевантный ресурс где-то в паттерне (with_min-common-choice, $Time, $Body)
			RDOPATPattern* pat = parser->getLastPATPattern();
			const RDORelevantResource* const rel = pat->findRelevantResource( res_name_src_info.src_text() );
			if ( !pat->currRelRes ) {
				// Внутри with_min-common-choice или $Time
				if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_NonExist || rel->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
					parser->error( res_name_src_info, rdo::format("Релевантный ресурс не может быть использован, т.к. он еще не существует: %s", rel->getName().c_str()) );
				}
			} else {
				// Внутри $Body
				// Проверяем использование неинициализированного рел.ресурса (rel) в Choice from другом рел.ресурсе (pat->currRelRes)
				if ( pat->currRelRes->isChoiceFromState() ) {
					if ( !rel->alreadyHaveConverter && !rel->isDirect() ) {
						parser->error( res_name_src_info, rdo::format("Релевантный ресурс неопределен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from", rel->getName().c_str()) );
					}
					if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
						parser->error( res_name_src_info, rdo::format("Релевантный ресурс в начале операции не существует (NonExist): %s", rel->getName().c_str()) );
					}
					if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
						parser->error( res_name_src_info, rdo::format("Сразу после создания (Create) релевантный ресурс '%s' можно использовать только в конверторах, но не в условии выбора", rel->getName().c_str()) );
					}
				}
				// Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
				if ( rel->getType()->isTemporary() ) {
					// В конверторе начала
					if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin ) {
						if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_Create && !rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->getName().c_str()) );
						}
						if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_Erase && rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->getName().c_str()) );
						}
						if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->getName().c_str()) );
						}
					}
					// В конверторе конца
					if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd ) {
						if ( rel->end == rdoRuntime::RDOResourceTrace::CS_Create && !rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->getName().c_str()) );
						}
						if ( rel->end == rdoRuntime::RDOResourceTrace::CS_Erase && rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->getName().c_str()) );
						}
						if ( rel->end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->getName().c_str()) );
						}
					}
				}
				// Проверяем использование еще не инициализированного (только для Create) параметра рел. ресурса в его же конверторе
				const RDORTPParam* param = pat->currRelRes->getType()->findRTPParam( par_name_src_info.src_text() );
				if ( param && pat->currRelRes->getName() == res_name_src_info.src_text() ) {
					// В конверторе начала
					if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin && pat->currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create) {
						if ( !pat->currRelRes->getParamSetBegin()->isExist( par_name_src_info.src_text() ) ) {
							if ( !param->getType()->dv->isExist() ) {
								parser->error( par_name_src_info, rdo::format("Перед использованием параметра '%s' ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name_src_info.src_text().c_str()) );
							}
						}
					}
					// В конверторе начала
					if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd && pat->currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create) {
						if ( !pat->currRelRes->getParamSetEnd()->isExist( par_name_src_info.src_text() ) ) {
							if ( !param->getType()->dv->isExist() ) {
								parser->error( par_name_src_info, rdo::format("Перед использованием параметра '%s' ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name_src_info.src_text().c_str()) );
							}
						}
					}
				}
			}
			int relResNumb = pat->findRelevantResourceNum( res_name_src_info.src_text() );
			int parNumb    = rel->getType()->getRTPParamNumber( par_name_src_info.src_text() );
			if ( parNumb == -1 ) {
				parser->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
//				parser->error( "Unknown resource parameter: " + *par_name_src_info.src_text() );
			}
			calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser->runTime, relResNumb, parNumb );
			calc->setSrcInfo( src_info() );
			type = rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
			if ( type == RDORTPParamType::pt_enum ) {
				enu = ((RDORTPEnumParamType*)rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
			}
			return;
		} else if ( parser->getFileToParse() == rdoModelObjects::DPT && parser->getLastDPTSearch() && parser->getLastDPTSearch()->lastActivity && parser->getLastDPTSearch()->lastActivity->getRule() && parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource( res_name_src_info.src_text() ) ) {
			// Это ресурс, который используется в DPT (condition, term_condition, evaluate_by, value before, value after)
			const RDORelevantResource* const rel = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource( res_name_src_info.src_text() );
			int relResNumb = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResourceNum( res_name_src_info.src_text() );
			int parNumb    = rel->getType()->getRTPParamNumber( par_name_src_info.src_text() );
			if ( parNumb == -1 ) {
				parser->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
			}

			calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser->runTime, relResNumb, parNumb );
			calc->setSrcInfo( src_info() );
			type = rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
			if ( type == RDORTPParamType::pt_enum ) {
				enu = ((RDORTPEnumParamType *)rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
			}
			return;
		}
	}
	parser->error( res_name_src_info, rdo::format("Неизвестный ресурс: %s", res_name_src_info.src_text().c_str()) );
//	parser->error(("Unknown resource name: " + *res_name_src_info.src_text()).c_str());
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, int value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runTime, value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, int value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runTime, value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, double* value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_real ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runTime, *value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, double* value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_real ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runTime, *value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, const std::string& value, const YYLTYPE& _pos ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( value, _pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, const std::string& value, const YYLTYPE& _pos ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( value, _pos );
}

void RDOFUNArithm::init( const std::string& value, const YYLTYPE& _pos )
{
	setSrcPos( _pos );
	setSrcText( value );

	if ( value == "Time_now" || value == "time_now" || value == "Системное_время" || value == "системное_время" ) {
		type = RDORTPParamType::pt_real;
		calc = new rdoRuntime::RDOCalcGetTimeNow( parser->runTime );
		calc->setSrcInfo( src_info() );
		return;
	}

	if ( value == "Seconds" || value == "seconds" ) {
		type = RDORTPParamType::pt_real;
		calc = new rdoRuntime::RDOCalcGetSeconds( parser->runTime );
		calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем параметры паттерна или функции по имени
	const RDOFUNFunctionParam* param = NULL;
	switch ( parser->getFileToParse() ) {
		case rdoModelObjects::PAT: param = parser->getLastPATPattern()->findPATPatternParam( value ); break;
		case rdoModelObjects::FUN: param = parser->getLastFUNFunction()->findFUNFunctionParam( value ); break;
	}

	// Ищем константы по имени
	const RDOFUNConstant* cons = parser->findFUNConst( value );

	if ( cons && param ) {
		parser->lexer_loc_set( src_info().src_pos().last_line, src_info().src_pos().last_pos );
		parser->error( rdo::format("Имя параметра образца совпадает с именем константы: %s", value.c_str()) );
//		parser->error( "Ambiguity: constant or parameter usage: " + *value + " ?" );
	}

	if ( cons ) {
		type = cons->getType()->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = ((RDORTPEnumParamType *)cons->getType())->enu;
		}
		calc = new rdoRuntime::RDOCalcGetConst( parser->runTime, cons->number );
		calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем последовательность по имени
	const RDOFUNSequence* seq = parser->findSequence( value );
	if ( seq && param ) {
		parser->lexer_loc_set( src_info().src_pos().last_line, src_info().src_pos().last_pos );
		parser->error( rdo::format("Имя параметра образца совпадает с именем последовательности: %s", value.c_str()) );
//		parser->error("Ambiguity: sequence or parameter usage: " + *value + " ?");
	}

	if ( seq ) {
		RDOFUNParams tmp( seq );
		RDOFUNArithm* arithm = tmp.createSeqCall( value );
		arithm->setSrcInfo( src_info() );
		type = arithm->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = arithm->enu;
		}
		calc = arithm->calc;
		return;
	}

	if ( param ) {
		// Это параметр
		type = param->getType()->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = ((RDORTPEnumParamType *)param->getType())->enu;
		}
		switch ( parser->getFileToParse() ) {
			case rdoModelObjects::PAT: calc = new rdoRuntime::RDOCalcPatParam( parser->runTime, parser->getLastPATPattern()->findPATPatternParamNum( value ) ); break;
			case rdoModelObjects::FUN: calc = new rdoRuntime::RDOCalcFuncParam( parser->runTime, parser->getLastFUNFunction()->findFUNFunctionParamNum( value ) ); break;
		}
		if ( calc ) calc->setSrcInfo( src_info() );
		return;
	} else {
		// Возможно, что это значение перечислимого типа
		std::vector< RDORTPEnumParamType* >::const_iterator it = parser->getEnums().begin();
		while ( it != parser->getEnums().end() ) {
			if ( (*it)->enu->findValue( value, false ) != -1 ) {
				// Да, это перечислимый тип, только одно и тоже значение может встречаться в разных
				// перечислимых типах, поэтому какой именно из них выбрать - вопрос
				type = RDORTPParamType::pt_unknow;
				str  = value;
				setSrcText( value );
				return;
			}
			it++;
		}
		parser->lexer_loc_set( _pos.last_line, _pos.last_column );
		parser->error( rdo::format("Неизвестный идентификатор: %s", value.c_str()) );
//		type = RDORTPParamType::pt_unknow;
//		str  = value;
//		return;
	}
}

RDOFUNArithm* RDOFUNArithm::operator +( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя складывать перечислимые данные" );
//		parser->error("cannot add enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}

	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, new rdoRuntime::RDOCalcPlus( parser->runTime, calc, second.calc ), src_pos() );
	arithm->setSrcInfo( src_info(), " + ", second.src_info() );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator -( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя вычитать перечислимые данные" );
//		parser->error("cannot subtract enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}

	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, new rdoRuntime::RDOCalcMinus( parser->runTime, calc, second.calc ), src_pos() );
	arithm->setSrcInfo( src_info(), " - ", second.src_info() );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator *( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя перемножать перечислимые данные" );
//		parser->error("cannot multiply enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcMult( parser->runTime, calc, second.calc );
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, src_pos() );
	arithm->setSrcInfo( src_info(), " * ", second.src_info() );
	newCalc->setSrcInfo( arithm->src_info() );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator /( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя разделить перечислимые данные" );
//		parser->error("cannot divide enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcDiv( parser->runTime, calc, second.calc );
	rdoRuntime::RDOCalc* newCalc_div = newCalc;
	// TODO: перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	if ( newType == RDORTPParamType::pt_int ) {
		newCalc = new rdoRuntime::RDOCalcDoubleToInt( parser->runTime, newCalc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, src_pos() );
	arithm->setSrcInfo( src_info(), " / ", second.src_info() );
	newCalc->setSrcInfo( arithm->src_info() );
	newCalc_div->setSrcInfo( arithm->src_info() );
	return arithm;
}

RDOFUNLogic* RDOFUNArithm::operator <( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
//		parser->error("cannot compare enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsLess( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsGreater( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator <=( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsLEQ( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >=( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsGEQ( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator ==( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_enum ) {
		if ( enu != second.enu ) {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( "Нельзя сравнивать разные перечислимые типы" );
//			parser->error("cannot compare different enumerative types");
		}
	} else if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_unknow ) {
		second.calc = new rdoRuntime::RDOCalcConst( parser->runTime, enu->findValue( second.str ) );
		second.calc->setSrcInfo( second.src_info() );
//	} else if ( (type >= 2) || (second.type >= 2) ) {
	} else if ( (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_enum) || (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_unknow) ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
//		parser->error("cannot compare enumerative type with nonenumerative type");
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsEqual( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator !=( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_enum ) {
		if ( enu != second.enu ) {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( "Нельзя сравнивать разные перечислимые типы" );
		}
	} else if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_unknow ) {
		second.calc = new rdoRuntime::RDOCalcConst( parser->runTime, enu->findValue( second.str ) );
		second.calc->setSrcInfo( second.src_info() );
	} else if ( (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_enum) || (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_unknow) ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcIsNotEqual( parser->runTime, calc, second.calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

rdoRuntime::RDOCalc* RDOFUNArithm::createCalc( const RDORTPParamType* const forType )
{
	if ( type != RDORTPParamType::pt_unknow ) {
		if ( forType == NULL ) {
			return calc;
		}
		if ( forType->getType() != RDORTPParamType::pt_int ) {
			return calc;
		}
		rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcInt( parser->runTime, calc );
		newCalc->setSrcInfo( src_info() );
		return newCalc;
	} else if ( type == RDORTPParamType::pt_unknow && !forType && !str.empty() ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format( "Неизвестный идентификатор: %s", str.c_str()) );
	}

	if ( !forType ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( "Неизвестный тип параметра" );
	}

	parser->lexer_loc_backup();
	parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcConst( parser->runTime, forType->getRSSEnumValue(str) );
	parser->lexer_loc_restore();
	newCalc->setSrcInfo( src_info() );
	return newCalc;
}

void RDOFUNArithm::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( calc ) calc->setSrcInfo( src_info );
}

void RDOFUNArithm::setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end )
{
	RDOParserSrcInfo::setSrcInfo( begin, delim, end );
	if ( calc ) calc->setSrcInfo( begin, delim, end );
}

void RDOFUNArithm::setSrcPos( const YYLTYPE& _error_pos )
{
	RDOParserSrcInfo::setSrcPos( _error_pos );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNArithm::setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
{
	RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNArithm::setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
{
	RDOParserSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNArithm::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( calc ) calc->setSrcText( src_info().src_text() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNParams::createCall( const std::string& funName ) const
{
	const RDOFUNFunction* const func = parser->findFunction( funName );
	if ( !func ) {
		return createSeqCall( funName );
	}

	int nParams = func->getParams().size();
	if ( nParams != params.size() ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format("Неверное количество параметров функции: %s", funName.c_str()) );
//		parser->error(("Wrong parameters number in function call: " + *funName).c_str());
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runTime, func->getFunctionCalc() );
	funcCall->setSrcInfo( src_info() );
	for ( int i = 0; i < nParams; i++ ) {
		const RDORTPParamType* const funcParam = func->getParams()[i]->getType();
		RDOFUNArithm* arithm = params[i];
		switch ( funcParam->getType() ) {
			case RDORTPParamType::pt_int: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if ( arithm->type == RDORTPParamType::pt_real ) {
					arg = new rdoRuntime::RDOCalcDoubleToInt( parser->runTime, arg );
				}
				if ( static_cast<const RDORTPIntParamType*>(funcParam)->diap->isExist() ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( parser->runTime, static_cast<const RDORTPIntParamType*>(funcParam)->diap->minVal, static_cast<const RDORTPIntParamType*>(funcParam)->diap->maxVal, arg );
				}
				funcCall->addParameter( arg );
				break;
			}
			case RDORTPParamType::pt_real: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if( static_cast<const RDORTPRealParamType*>(funcParam)->diap->isExist() ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( parser->runTime, static_cast<const RDORTPRealParamType*>(funcParam)->diap->minVal, static_cast<const RDORTPRealParamType*>(funcParam)->diap->maxVal, arg );
				}
				funcCall->addParameter(arg);
				break;
			}
			default: {
				if ( arithm->type == RDORTPParamType::pt_int || arithm->type == RDORTPParamType::pt_real ) {
					parser->lexer_loc_set( arithm->src_pos().last_line, arithm->src_pos().last_pos );
					parser->error( "Неверный тип параметра функции" );
//					parser->error("wrong parameter type");
				}

				rdoRuntime::RDOCalc* arg;
				if ( arithm->type == RDORTPParamType::pt_enum ) {
					if ( arithm->enu != static_cast<const RDORTPEnumParamType*>(funcParam)->enu ) {
						parser->error( "Перечислимые типы не совпадают" );
//						parser->error("wrong enumerative parameter type");
					}
					arg = arithm->createCalc( NULL );
				} else {
					arg = new rdoRuntime::RDOCalcConst( parser->runTime, static_cast<const RDORTPEnumParamType*>(funcParam)->enu->findValue(arithm->str) );
					arg->setSrcInfo( arithm->src_info() );
				}
				funcCall->addParameter(arg);
				break;
			}
		}
	}

	RDOFUNArithm* res = new RDOFUNArithm( this, RDORTPParamType::pt_int, funcCall, src_pos() );
	res->type = func->getType()->getType();
	if ( func->getType()->getType() == RDORTPParamType::pt_enum ) {
		res->enu = ((RDORTPEnumParamType *)func->getType())->enu;
	}
	res->setSrcInfo( src_info() );

	return res;
}

RDOFUNArithm* RDOFUNParams::createSeqCall( const std::string& seqName ) const
{
	const RDOFUNSequence* const seq = parser->findSequence( seqName );
	if ( !seq ) {
		parser->lexer_loc_set( name_error_pos.src_pos().last_line, name_error_pos.src_pos().last_pos );
		parser->error( rdo::format("Неопределенная функция или последовательность: %s", seqName.c_str()) );
//		parser->error( "Undefined function or sequence: " + *seqName );
	}
	RDOFUNArithm* arithm = seq->createCallCalc( this, src_info() );
	return arithm;
}

void RDOFUNFunction::createAlgorithmicCalc()
{
	rdoRuntime::RDOFunAlgorithmicCalc *funcCalc = new rdoRuntime::RDOFunAlgorithmicCalc( parser->runTime );
	int size = calculateIf.size();
	if ( !size ) {
		parser->warning( "Отсутствует тело функции" );
	}
	for ( int i = 0; i < size; i++ ) {
		funcCalc->addCalcIf(calculateIf[i]->condition->calc, calculateIf[i]->action->createCalc(getType()));
	}
	functionCalc = funcCalc;
}

RDOFUNCalculateIf::RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, const std::string& _funName, RDOFUNArithm* _action ):
	RDOParserObject( _parent ),
	condition( _condition ),
	funName( _funName ),
	action( _action )
{
	if( funName != parser->getLastFUNFunction()->getName() ) {
		parser->error( rdo::format("Ожидается имя функции: %s", parser->getLastFUNFunction()->getName().c_str()) );
//		parser->error( "function name expected" );
	}
	parser->getLastFUNFunction()->getType()->checkParamType( action );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup( RDOParser* _parser, const std::string& _resType ):
	RDOParserObject( _parser )
{
	init( _resType );
}

RDOFUNGroup::RDOFUNGroup( const RDOParserObject* _parent, const std::string& _resType ):
	RDOParserObject( _parent )
{
	init( _resType );
}

void RDOFUNGroup::init( const std::string& _resType )
{
	resType = parser->findRTPResType( _resType );
	if ( !resType ) {
		parser->error( rdo::format("Неизвестный тип ресурса: %s", _resType.c_str()) );
	}
	parser->insertFUNGroup( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
RDOFUNLogic* RDOFUNGroupLogic::createFunLogic( RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcGroup* calc;
	switch ( funType ) {
		case 1 : setSrcText( "Exist(" + resType->getName() + ": " + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcExist    ( parser->runTime, resType->getNumber(), cond->calc ); break;
		case 2 : setSrcText( "NotExist(" + resType->getName() + ": " + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcNotExist ( parser->runTime, resType->getNumber(), cond->calc ); break;
		case 3 : setSrcText( "ForAll(" + resType->getName() + ": " + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcForAll   ( parser->runTime, resType->getNumber(), cond->calc ); break;
		case 4 : setSrcText( "NotForAll(" + resType->getName() + ": " + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcNotForAll( parser->runTime, resType->getNumber(), cond->calc ); break;
		default: parser->error( "Несуществующий тип функции" );
	}
	parser->getFUNGroupStack().pop_back();
	calc->setSrcInfo( src_info() );
	RDOFUNLogic* logic = new RDOFUNLogic( calc );
	logic->setSrcInfo( src_info() );
	return logic;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
// Сам Select как выборка по типу и условию
RDOFUNLogic* RDOFUNSelect::createFunSelect( RDOFUNLogic* cond )
{
	select = new rdoRuntime::RDOFunCalcSelect( parser->runTime, resType->getNumber(), cond ? cond->calc : new rdoRuntime::RDOCalcConst(parser->runTime, 1) );
	return new RDOFUNLogic( select );
}

// Групповая функция над выборкой Select'а
RDOFUNLogic* RDOFUNSelect::createFunSelectGroup( int funType, RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcSelectBase* calc;
	switch ( funType ) {
		case 1 : setSrcText( src_text() + ".Exist(" + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcSelectExist    ( parser->runTime, select, cond->calc ); break;
		case 2 : setSrcText( src_text() + ".NotExist(" + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcSelectNotExist ( parser->runTime, select, cond->calc ); break;
		case 3 : setSrcText( src_text() + ".ForAll(" + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcSelectForAll   ( parser->runTime, select, cond->calc ); break;
		case 4 : setSrcText( src_text() + ".NotForAll(" + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcSelectNotForAll( parser->runTime, select, cond->calc ); break;
		default: parser->error( "Неизвестный метод для списка ресурсов" );
	}
	parser->getFUNGroupStack().pop_back();
	calc->setSrcInfo( src_info() );
	RDOFUNLogic* logic = new RDOFUNLogic( calc );
	logic->setSrcInfo( src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNSelect::createFunSelectEmpty()
{
	setSrcText( src_text() + ".Empty()" );
	parser->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( select );
	logic->setSrcInfo( src_info() );
	return logic;
}

RDOFUNArithm* RDOFUNSelect::createFunSelectSize()
{
	setSrcText( src_text() + ".Size()" );
	parser->getFUNGroupStack().pop_back();
	RDOFUNArithm* arithm = new RDOFUNArithm( this, RDORTPParamType::pt_int, new rdoRuntime::RDOFunCalcSelectSize( parser->runTime, select ), src_pos() );
	arithm->setSrcInfo( src_info() );
	return arithm;
}

void RDOFUNSelect::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( select ) select->setSrcInfo( src_info );
}

void RDOFUNSelect::setSrcPos( const YYLTYPE& _error_pos )
{
	RDOParserSrcInfo::setSrcPos( _error_pos );
	if ( select ) select->setSrcPos( src_info().src_pos() );
}

void RDOFUNSelect::setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
{
	RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	if ( select ) select->setSrcPos( src_info().src_pos() );
}

void RDOFUNSelect::setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
{
	RDOParserSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
	if ( select ) select->setSrcPos( src_info().src_pos() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOParserObject( _parser ),
	header( _header ),
	base( _base )
{
	parser->insertFUNSequences( this );
}

void RDOFUNSequence::initResult()
{
	initCalcSrcInfo();
	switch ( header->type->getType() ) {
		case RDORTPParamType::pt_int: {
			next->res_real = false;
			if ( static_cast<RDORTPIntParamType*>(header->type)->diap->isExist() ) {
				next->diap     = true;
				next->diap_min = static_cast<RDORTPIntParamType*>(header->type)->diap->minVal;
				next->diap_max = static_cast<RDORTPIntParamType*>(header->type)->diap->maxVal;
			}
			break;
		}
		case RDORTPParamType::pt_real: {
			if ( static_cast<RDORTPRealParamType*>(header->type)->diap->isExist() ) {
				next->diap     = true;
				next->diap_min = static_cast<RDORTPRealParamType*>(header->type)->diap->minVal;
				next->diap_max = static_cast<RDORTPRealParamType*>(header->type)->diap->maxVal;
			}
			break;
		}
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	initSeq->setSrcInfo( header->src_info() );
	initSeq->setSrcText( "Инициализация последовательности " + initSeq->src_text() );
	next->setSrcInfo( header->src_info() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPParamType::pt_enum ) {
		parser->error( "Последовательность типа uniform не может иметь перечислимый тип" );
//		parser->error("Uniform sequence cannot be enumerative type");
	}
	createCalcs();
}

void RDOFUNSequenceUniform::createCalcs()
{
	rdoRuntime::RandGeneratorUniform* gen = new rdoRuntime::RandGeneratorUniform();
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runTime, base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextUniform( parser->runTime, gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceUniform::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона", header->name.c_str()) );
//		parser->error("Wrong parameters number in uniform sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runTime, next );
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceUniform*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( "Внутренняя ошибка парсера" );
//		parser->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader *_header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPParamType::pt_enum ) {
		parser->error( "Последовательность типа exponential не может иметь перечислимый тип" );
//		parser->error("Exponential sequence cannot be enumerative type");
	}

	createCalcs();
}

void RDOFUNSequenceExponential::createCalcs()
{
	rdoRuntime::RandGeneratorExponential* gen = new rdoRuntime::RandGeneratorExponential();
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runTime, base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextExponential( parser->runTime, gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceExponential::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 1 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание", header->name.c_str()) );
//		parser->error("Wrong parameters number in exponential sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runTime, next );
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);

	funcCall->addParameter(arg1);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceExponential*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( "Внутренняя ошибка парсера" );
//		parser->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPParamType::pt_enum ) {
		parser->error( "Последовательность типа normal не может иметь перечислимый тип" );
//		parser->error("Normal sequence cannot be enumerative type");
	}
	createCalcs();
}

void RDOFUNSequenceNormal::createCalcs()
{
	rdoRuntime::RandGeneratorNormal* gen = new rdoRuntime::RandGeneratorNormal();
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runTime, base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextNormal( parser->runTime, gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceNormal::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение", header->name.c_str()) );
//		parser->error("Wrong parameters number in normal sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(parser->runTime, next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceNormal*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( "Внутренняя ошибка парсера" );
//		parser->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceByHist::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 0 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Гистограмма '%s' должна вызываться без параметров", header->name.c_str()) );
//		parser->error("Wrong parameters number in by_hist sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runTime, next );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceByHist*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		arithm->enu = ((RDORTPEnumParamType *)header->type)->enu;
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistReal::addReal( double _from, double _to, double _freq )
{
	from.push_back( _from );
	to.push_back( _to );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	rdoRuntime::RandGeneratorByHist *gen = new rdoRuntime::RandGeneratorByHist();
	int size = from.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues(from[i], to[i], freq[i]);
	}

	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runTime, base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runTime, gen);
	initCalcSrcInfo();
	initResult();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistEnum::addEnum( const std::string& _val, double _freq )
{
	rdoRuntime::RDOValue enum_id = header->type->getRSSEnumValue( _val );
	if ( std::find( val.begin(), val.end(), enum_id ) != val.end() ) {
		parser->error( rdo::format("Перечислимое значение определено дважды: %s", _val.c_str()) );
	}
	val.push_back( enum_id );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	rdoRuntime::RandGeneratorByHistEnum* gen = new rdoRuntime::RandGeneratorByHistEnum();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues( val[i], freq[i] );
	}
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runTime, base, gen );
	parser->runTime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextByHist( parser->runTime, gen );
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceEnumerative::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 0 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Перечисление '%s' должно вызываться без параметров", header->name.c_str()) );
//		parser->error("Wrong parameters number in enumerative sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(parser->runTime, next);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceEnumerative*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		arithm->enu = ((RDORTPEnumParamType *)header->type)->enu;
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeInt
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeInt::addInt( int _val )
{
	val.push_back(_val);
}

void RDOFUNSequenceEnumerativeInt::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative* gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValue(val[i]);
	}
	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runTime, base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runTime, gen);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeReal::addReal( double* _val )
{
	val.push_back( *_val );
}

void RDOFUNSequenceEnumerativeReal::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative *gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runTime, base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runTime, gen);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeEnum::addEnum( const std::string& _val )
{
	val.push_back( header->type->getRSSEnumValue(_val) );
}

void RDOFUNSequenceEnumerativeEnum::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative *gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runTime, base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runTime, gen);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant( RDOParser* _parser, RDORTPParam* _param ):
	RDOParserObject( _parser ),
	param( _param ),
	number( _parser->getFUNCONST_id() )
{
	parser->insertFUNConstant( this );
}

} // namespace rdoParse
